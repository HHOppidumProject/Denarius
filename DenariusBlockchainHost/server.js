'use strict';
var http = require('http');
var ws = require('ws');
var uuid = require('uuid');
const { Worker } = require('worker_threads')

var port = process.env.PORT || 3000;
var hostname = 'localhost';

var nodeIdentifier = uuid.v4();
nodeIdentifier = nodeIdentifier.replace(/-/g, '')
console.log(nodeIdentifier);

const worker = new Worker('./blockchain.js', { workerData: { i: 0 } });

worker.on("message", function (data) {
    console.log(data);
})
var Blockchain = "";

http.createServer(function (req, res) {
    res.writeHead(200, { 'Content-Type': 'text/plain' });

    const sock = new ws.WebSocket('ws://localhost:3001');

    var auth = "Hello"

    sock.on('open', function open() {
        sock.send('Awaiting Blockchain');
    });

    sock.on('message', function incoming(message) {
        console.log('received: %s', message);
        Blockchain = message;
    });

    console.log("Hello!");
    res.write(Blockchain);
    res.end("\nHello world!");

}).listen(port, hostname, () => { console.log(`Server running at http://${hostname}:${port}/`); });
