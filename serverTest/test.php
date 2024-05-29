<?php
use Workerman\Worker;
use Workerman\Connection\TcpConnection;
require_once __DIR__ . '/vendor/autoload.php';

// #### MyTextProtocol worker ####
$text_worker = new Worker("MyTextProtocol://0.0.0.0:2333");
$global_received_bytes = 0;
$start_time = time();
// // tcp server
// $tcp_worker = new Worker("tcp://0.0.0.0:5001");

// // 当客户端连接时
// $tcp_worker->onConnect = function($connection)
// {
//     echo "New Connection\n";
// };

// // 当客户端发送消息过来时，转发给所有客户端
// $tcp_worker->onMessage = function($connection, $data) use ($tcp_worker)
// {
//     foreach($tcp_worker->connections as $client_connection)
//     {
//         $client_connection->send($data);
//     }
// };

// // 当客户端连接断开时
// $tcp_worker->onClose = function($connection)
// {
//     echo "Connection closed\n";
// };


/*
 * 收到一个完整的数据（结尾是换行）后，自动执行MyTextProtocol::decode('收到的数据')
 * 结果通过$data传递给onMessage回调
 */
// $text_worker->onMessage =  function(TcpConnection $connection, $data)
// {
//     var_dump($data);
//     /*
//      * 给客户端发送数据，会自动调用MyTextProtocol::encode('hello world')进行协议编码，
//      * 然后再发送到客户端
//      */
//     $connection->send("hello world");
// };


// 当客户端连接时
$text_worker->onConnect = function($connection)
{
    echo "New Connection\n";
};

// 当客户端发送消息过来时，转发给所有客户端
$text_worker->onMessage = function($connection, $data) use ($text_worker, &$global_received_bytes, &$start_time)
{
    $global_received_bytes += strlen($data);
    // 计算当前时间
    $current_time = time();
    // 计算经过的时间（秒）
    $elapsed_time = $current_time - $start_time;
    if ($elapsed_time > 0) {
        $speed = $global_received_bytes / $elapsed_time * 8;
        echo "Speed: {$speed} bits/sec, rec={$global_received_bytes}, elatime={$elapsed_time}, cur={$current_time}, start={$start_time}\n";

        // 重置统计（根据需要可以注释掉下面两行来累计更长时间的数据）
        $global_received_bytes = 0;
        $start_time = time();
    }

    foreach($text_worker->connections as $client_connection)
    {
        $client_connection->send($data);
    }
};

// 当客户端连接断开时
$text_worker->onClose = function($connection)
{
    echo "Connection closed\n";
};
// run all workers
Worker::runAll();