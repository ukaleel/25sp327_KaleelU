childPID=$(pgrep -x child)

if [ -z "$childPID" ]; then
    echo "No child process found."
    exit 1
fi

echo "Sending SIGTSTP to child process (PID: $childPID) every 3 seconds..."
for i in {1..5}; do
    kill -SIGTSTP $childPID
    sleep 3
done

echo "Done sending signals. Now sending SIGTERM to child."
kill -SIGTERM $childPID
