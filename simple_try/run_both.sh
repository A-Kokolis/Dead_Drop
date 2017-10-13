#!/bin/bash

rm /tmp/sender_pipe
rm /tmp/receiver_pipe

sender_pipe='/tmp/sender_pipe'
receiver_pipe='/tmp/receiver_pipe'


mkfifo $sender_pipe
mkfifo $receiver_pipe

echo "Starting both receiver and sender"

taskset -c 1 ./receiver >recv_out <<< '\n' & 
taskset -c 5 ./sender > /dev/null 2>&1 <<< '\n' &

echo "Both of them started"
echo "Sending an enter to stdin to set up the covert channel"
sleep 1

echo "Write for sender"

read message



wait

echo "Done"


#echo '\n' > "$sender_pipe"
#echo '\n' > "$receiver_pipe"


