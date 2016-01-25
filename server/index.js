var express = require('express');
var app = express();
var fs = require('fs');
var bodyParser = require('body-parser');
var mongo = require('mongodb');
var mongoose = require('mongoose');
var db = mongoose.connection;

//routing sources
var user = require('./routes/user');


//routes
app.use('/user', user);

app.set('port', (process.env.PORT || 5000));

app.use(express.static(__dirname + '/public'));

// views is directory for all template files
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');

app.get('/', function(request, response) {
  response.render('pages/index');
});

/*
  returns a json object with the roast types
 */
app.get('/rtypes', function(req,res) {
  var roasttypeList = fs.readFileSync("roastTypes.json");
  res.json(JSON.parse(roasttypeList));
});

app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});


