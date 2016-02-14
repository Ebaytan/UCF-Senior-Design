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
                res.json({success: false, msg: 'Username already exists.'});
            } else {
                res.json({success: true, msg: 'Successful created user!'});
            }
        });
    }
});

router.post('/authenticate', function (req, res) {
    User.findOne({
        username: req.body.username
    }, function (err, user) {
        if (err) throw err;

        if (!user) {
            res.send({success: false, msg: 'Authentication failed. User not found.'});
        } else {
            user.comparePassword(req.body.password, function (err, isMatch) {
                if (isMatch && !err) {
                    var token = jwt.encode(user, config.secret);
                    res.json({success: true, token: 'JWT ' + token});
                } else {
                    res.send({success: false, msg: 'Authentication failed. Wrong password.'});
                }
            });
        }
    });
});

router.get('/memberinfo', passport.authenticate('jwt', {session: false}), function (req, res) {
    var token = getToken(req.headers);
    if (token) {
        var decoded = jwt.decode(token, config.secret);
        User.findOne({
            username: decoded.username
        }, function (err, user) {
            if (err) throw err;

            if (!user) {
                return res.status(403).send({success: false, msg: 'Authentication failed. User not found.'});
            } else {
                return res.json({success: true, msg: 'Welcome in the member area ' + user.username + '!'});
            }
        });
    } else {
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