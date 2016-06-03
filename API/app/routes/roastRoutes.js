var express = require('express');
var router = express.Router();
var mongoose = require('mongoose');
var passport = require('passport');
var config = require('../../config/database'); // get db config file
var Roast = require('../models/roastModel'); // get the mongoose model
var User = require('../models/user');
require('../../config/passport')(passport);
var jwt = require('jwt-simple');

/**
 * Roasts with base URI of /api/roast/
 */
router.post('/', function (req, res) {
    //grab authentication token sent from the client that was attached to header
    var token = getToken(req.headers);

    //does the token exist?
    if (token) {
        //make use of our secret which is simply our key to encrypt and decrypt token
        //then store that value in variable decoded
        var decoded = jwt.decode(token, config.secret);

        //ensure that data necessary to create roast profile has been provided
        if (!req.body.name || !req.body.roastType || !req.body.beanType || !req.body.RoastingData) {
            res.json({success: false, msg: 'Please fill all necessary fields.'});
        }
        else {
            var newRoast = new Roast({
                name: req.body.name,
                roastType: req.body.roastType,
                beanType: req.body.beanType,
                creatorID: decoded.username,  //value comes from token
                RoastingData: req.body.RoastingData,
                rating: 0   //initially 0
            });
            //Save roast in roasting profile document
            newRoast.save(function (err) {
                if (err) {
                    console.log(err);
                    res.json({success: false, error: err});
                }

                //update users roasting profiles array
                User.update(
                    decoded.username,
                    {
                        $push: {
                            roastingprofiles: req.body.name
                        }
                    },
                    {
                        upsert: true
                    },
                    function (err) {
                        if (err) {
                            console.log(err);
                            res.json({success: false, error: err});
                        }
                        else {
                            res.json({success: true, msg: 'Roast profile successfully created!'})
                        }
                    }
                );

            });
        }
        //no token was found on the header of the request
    } else {
        //error response
        return res.status(403).send({success: false, msg: 'No token provided.'});
    }
});

/**
 * Returns a list of all roasting profiles in the database
 */
router.get('/', function (req, res) {

    Roast.find({}, function (err, roasts) {

        if (err) {
            console.log(err);
            res.json({success: false, error: err});
        }

        res.json({success: true, roasts: roasts});
    });

});

/**
 * Returns a specific roast profile
 *
 * Note:
 *     If the name of the roast is multiple words do not
 *     wrap in quotes send as is.
 */
router.get('/:name', function (req, res) {

    Roast.find({name: req.params.name}, function (err, roast) {

        if (err) {
            console.log(err);
            res.json({success: false, error: err});
        }

        res.json({success: true, roast: roast});

    });

});

router.post('/:name/rateup', function (req, res) {

    Roast.update(
        {
            name: req.params.name
        },
        {
            $inc: {
                rating: 1  //increment rating by 1
            }
        },
        {
            upsert: true
        },
        function (err) {
            if (err) {
                console.log(err);
                console.log(req.params.name);
                res.json({success: false, error: err});
            }
            else {
                console.log(req.params.name);
                res.json({success: true, msg: 'Roast profile successfully rated up!'})
            }
        }
    );

});

router.post('/:name/ratedown', function (req, res) {
    Roast.update(
        {
            name: req.params.name
        },
        {
            $inc: {
                rating: -1  //decrement rating by 1
            }
        },
        {
            upsert: true
        },
        function (err) {
            if (err) {
                console.log(err);
                console.log(req.params.name);
                res.json({success: false, error: err});
            }
            else {
                console.log(req.params.name);
                res.json({success: true, msg: 'Roast profile successfully rated down!'})
            }
        }
    );
});


/**
 * Query by type of Roast
 */
router.get('/type/:type', function (req, res) {
    Roast.find(
        {
            roastType: req.params.type
        },
        function (err, roasts) {
            if (err) {
                console.log(err);
                res.json({success: false, msg: err});
            }

            res.json({success: true, roasts: roasts});
        }
    );
});


module.exports = router;
