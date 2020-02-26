var express = require('express');
var path = require('path');
var mysql = require('./dbcon.js');
var bodyParser = require('body-parser');
var app = express();
var handlebars = require('express-handlebars').create({defaultLayout: 'main'});
var mysql = require('./dbcon.js');

app.use(bodyParser.urlencoded({extended: false}));
app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, '/public')));
app.use('/static', express.static('public'));
app.use('/search', require('./therapist.js'));
app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');
app.set('port', 54320);
app.set('mysql', mysql);


app.get('/', function(req, res)
{
  var qKey = [];
  for (var i in req.query){
    qKey.push({'key': i, 'value': req.query[i]})
  }
  //Array of items put into an object
  var content = {};
  content.data = qKey;
  //Call type declared
  content.callType = 'GET';
  //Post.handlebars served
  res.render('home', content);
});

//If user requests via POST items are sent to body
app.post('/', function(req, res)
{
  var keyB = [];
  for (var i in req.body){
    keyB.push({'key': i, 'value': req.body[i]});
  }
  var keyQ = [];
  for (var q in req.body){
    keyQ.push({'key': q, 'value': req.query[q]});
  }
  var content = {};
  content.Bdata = keyB;
  content.Qdata = keyQ;
  res.render('post', content);
})

app.get('/login', function(req,res)
{
  res.render('login');
});

app.get('/register', function(req,res)
{
  res.render('register');
});

app.use('/', require('./routes/zipSearch.js'));

//404 error
app.use(function(req, res){
	res.status(404);
	res.render("404");
});

//500 error
app.use(function(err, req, res, next){
	console.log(err.stack);
	res.status(500);
	res.render("500");
});

app.listen(app.get('port'), function(){
  console.log('Express started on http://flip3.engr.oregonstate.edu:' + app.get('port') + '; press Ctrl-C to terminate.');
});
