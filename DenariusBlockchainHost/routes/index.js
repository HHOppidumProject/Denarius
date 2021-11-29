var express = require('express');
var ws = require('ws');
var router = express.Router();


var app = express();

function GET(req, res, next) {

    var Blockchain = '';
    const sock = new ws.WebSocket('ws://localhost:3002');

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
        res.render('index', { title: 'Home', blockchain: BLOCKCHAINJSON, active: 'home' });
    });
}

/* GET home page. */
router.get('/', GET);

router.get('/index', GET);

router.get('/home', GET);

module.exports = router;
