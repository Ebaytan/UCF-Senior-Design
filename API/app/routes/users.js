var express = require('express');
var router = express.Router();
var mongoose = require('mongoose');
var passport = require('passport');
var config = require('../../config/database'); // get db config file
var User = require('../models/user'); // get the mongoose model
mongoose.connect(config.database);
require('../../config/passport')(passport);
var jwt = require('jwt-simple');

/*
 When roast state changes or update needed use update with the correct parameters
 */


router.post('/signup', function (req, res) {
    //make sure the required data was received
    if (!req.body.username || !req.body.password || !req.body.roasterUID || !req.body.email) {
        res.json({success: false, msg: 'Please fill all necessary fields.'});
    } else {

        //TODO: Check inputted data to make sure they are formatted correctly

        var newUser = new User({
            username: req.body.username,
            password: req.body.password,
            email: req.body.email,
            roaster: {
                roasterUID: req.body.roasterUID
            }
        });
        newUser.save(function (err) {
            if (err) {
                console.log(err);
                res.json({success: false, msg: 'Username already exists.'});
            } else {
                res.json({success: true, msg: 'Successful created user!'});
            }
        });
    }
});

router.post('/authenticate', function (req, res) {
    //find user with passed username
    User.findOne({
        username: req.body.username
    //once found
    }, function (err, user) {
        //throw error if one occured
        if (err) throw err;

        //user not found
        if (!user) {
            //response if user was not found
            res.send({success: false, msg: 'Authentication failed. User not found.'});
        } else {
            //compare stored hashed password to password that was passed
            user.comparePassword(req.body.password, function (err, isMatch) {
                //if good paassowrd
                if (isMatch && !err) {
                    //we encrypt user data using the secret (key) into a token
                    var token = jwt.encode(user, config.secret);
                    //send token to user
                    res.json({success: true, token: 'JWT ' + token});
                } else {
                    res.send({success: false, msg: 'Authentication failed. Wrong password.'});
                }
            });
        }
    });
});

router.get('/memberinfo', passport.authenticate('jwt', {session: false}), function (req, res) {
    //grab authentication token sent from the client that was attached to header
    var token = getToken(req.headers);

    //does the token exist?
    if (token) {
        //make use of our secret which is simply our key to encrypt and decrypt token
        //then store that value in variable decoded
        var decoded = jwt.decode(token, config.secret);
        //run a query on the database looking for user
        User.findOne({
            username: decoded.username
        //once complete
        }, function (err, user) {
            //did we run into a problem? if so throw an error
            if (err) throw err;
            //user does not exist?
            if (!user) {
                //error response to user
                return res.status(403).send({success: false, msg: 'Authentication failed. User not found.'});
            //we were successful in finding the user
            } else {
                //success response to user
                return res.json({success: true, msg: 'Welcome in the member area ' + user.username + '!'});
            }
        });
    //no token was found on the header of the request
    } else {
        //error response
        return res.status(403).send({success: false, msg: 'No token provided.'});
    }
});

getToken = function (headers) {
    if (headers && headers.authorization) {
        var parted = headers.authorization.split(' ');
        if (parted.length === 2) {
            return parted[1];
        } else {
            return null;
        }
    } else {
        return null;
    }
};

module.exports = router;