var express = require('express');
var app = express();
var fs = require('fs');

app.set('port', (process.env.PORT || 5000));

app.use(express.static(__dirname + '/public'));

// views is directory for all template files
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');

app.get('/', function(request, response) {
  response.render('pages/index');
});

app.get('/rtypes', function(req,res) {
  var roasttypeList = fs.readFileSync("roasttypes.json");
  res.json(JSON.parse(roasttypeList));
});

app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});


