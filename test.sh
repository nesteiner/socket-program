target/poll_server &

for i in {1..10}
do
    target/poll_client &
done
