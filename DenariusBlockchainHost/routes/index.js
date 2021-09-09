var express = require('express');
var ws = require('ws');
var router = express.Router();

/* GET home page. */
router.get('/', function GET(req, res, next) {

    var Blockchain = '';
    const sock = new ws.WebSocket('ws://localhost:3001');

    sock.on('error', function error(err) {
        console.log(err);
        GET(req, res, next);
    });

    sock.on('open', function open() {
        sock.send('Awaiting Blockchain');
    });

    sock.on('message', function incoming(message) {
        Blockchain = message;
        var BLOCKCHAINJSON = JSON.parse(Blockchain);
        res.render('index', { title: 'Blockchain', blockchain: Blockchain });
    });
});

module.exports = router;
