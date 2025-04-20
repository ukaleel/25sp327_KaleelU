#include <malloc.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>



#include "queue_internal.h"

// cmocka.h must be included after all other includes
#include <cmocka.h>

#define UNUSED(x) (void)(x)

typedef struct {
    int x;
} data_t;

int __wrap_pthread_mutex_lock(pthread_mutex_t* mtx) {
    UNUSED(mtx);
    return mock_type(int);
}

int __wrap_pthread_mutex_unlock(pthread_mutex_t* mtx) {
    UNUSED(mtx);
    return mock_type(int);
}

void test_queue_init(void** state) {
    UNUSED(state);

    // Arrange
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;

    // Act
    queue_t* que = queue_init(&mutex, &cond_var);

    // Assert
    assert_non_null(que);
    assert_ptr_equal(que->mutex, &mutex);
    assert_ptr_equal(que->cond_var, &cond_var);
    assert_false(que->isclosed);
    assert_int_equal(que->size, 0);
    assert_null(que->header);
    //assert_ptr_equal(que->tail);

    // cleanup
    free(que->header);
    free(que);
}

void test_queue_enqueue_when_q_empty(void** state) {
    UNUSED(state);

    // Arrange
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    queue_t* que = queue_init(&mutex, &cond_var);
    data_t data;
    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);

    // Act
    queue_enqueue(que, &data);

    // Assert
    assert_int_equal(que->size, 1);
    assert_ptr_equal(que->header, que->tail); \
    assert_ptr_equal(que->header->data, &data);
    assert_null(que->header->next); 

    // Cleanup
    free(que->header);
    free(que);
}

void test_queue_enqueue_when_q_not_empty(void** state){
    UNUSED(state);
    // Arrange
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    queue_t* que = queue_init(&mutex, &cond_var);
    data_t data1;
    data_t data2;

    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);



    queue_enqueue(que, &data1);  
    // Act

    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);

    queue_enqueue(que, &data2);
    // Assert
    assert_int_equal(que->size, 2);
    assert_ptr_equal(que->header->data, &data1);
    assert_ptr_equal(que->header->next->data, &data2);
    assert_ptr_equal(que->tail->data, &data2);
    assert_null(que->tail->next);
    // cleanup
    free(que->header->next);
    free(que->header);
    free(que);
    
}

void test_queue_dequeue(void** state){
    UNUSED(state);
    // Arrange
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    queue_t* que = queue_init(&mutex, &cond_var);
    data_t data1;
    data_t data2;

    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);


    queue_enqueue(que, &data1);  

    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);

    queue_enqueue(que, &data2);  

    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);

    // Act
    queue_dequeue(que);
    // Assert
    assert_int_equal(que->size, 1);
    assert_ptr_equal(que->header, que->tail);
    assert_ptr_equal(que->header->data, &data2);
    assert_null(que->header->next);

    free(que->header);
    free(que);
    //assert_ptr_equal(que->header->next, NULL);
}

void test_queue_dequeue_when_q_empty(void** state){
    UNUSED(state);
    // Arrange
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    queue_t* que = queue_init(&mutex, &cond_var);
    data_t data1;
    data_t data2;

    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);


    queue_enqueue(que, &data1);  


    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);

    queue_enqueue(que, &data2);  
    // Act

    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);
    queue_dequeue(que);
    // Assert
    assert_int_equal(que->size, 1);
    assert_ptr_equal(que->header, que->tail);
    assert_ptr_equal(que->header->data, &data2);
    assert_ptr_equal(que->header->next, NULL);


    free(que->header);
    free(que);

}


void test_queue_destroy(void **state){
    UNUSED(state);
    //Arrange
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    queue_t* que = queue_init(&mutex, &cond_var);


    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);

    //Act
    queue_destroy(que);
    //Assert
    assert_int_equal(que->size, 0);
    assert_ptr_equal(que->header, NULL);
    assert_ptr_equal(que->tail, NULL);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_queue_init),
        cmocka_unit_test(test_queue_enqueue_when_q_empty),
        cmocka_unit_test(test_queue_enqueue_when_q_not_empty),
        cmocka_unit_test(test_queue_dequeue),
        cmocka_unit_test(test_queue_dequeue_when_q_empty),
        // Add more tests here
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}