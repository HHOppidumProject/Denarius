var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
var uuid = require('uuid');
var eta = require('eta');
const { Worker } = require('worker_threads')

var indexRouter = require('./routes/index');
var blockchainRouter = require('./routes/blockchain');
var usersRouter = require('./routes/users');

var app = express();



var nodeIdentifier = uuid.v4();
nodeIdentifier = nodeIdentifier.replace(/-/g, '')
console.log(nodeIdentifier);

// Create Instance of blockchain

const worker = new Worker('./blockchain.js', { workerData: { i: 0 } });

worker.on("message", function (data) {
    console.log(data);
})

// view engine setup
app.engine("eta", eta.renderFile);
app.set('views', path.join(__dirname, 'views'));
app.set("view engine", "eta");

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use('/jquery', express.static(__dirname + '/node_modules/jquery/dist/'));
app.use('/select2', express.static(__dirname + '/node_modules/select2/dist/'));
app.use('/he', express.static(__dirname + '/node_modules/he/'));
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/index', indexRouter);
app.use('/home', indexRouter);
app.use('/blockchain', blockchainRouter);
app.use('/users', usersRouter);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
    res.render('error', {message: res.locals.message, error: res.locals.error, title: 'error'});
});

app.set('port', process.env.PORT || 3000);
app.listen(app.get('port'));


module.exports = app;
