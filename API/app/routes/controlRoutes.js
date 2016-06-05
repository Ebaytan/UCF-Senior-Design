var express = require('express');
var router = express.Router();
var mongoose = require('mongoose');
var passport = require('passport');
var config = require('../../config/database'); // get db config file
var Roast = require('../models/roastModel'); // get the mongoose model
var User = require('../models/user');
require('../../config/passport')(passport);
var jwt = require('jwt-simple');

//root uri /api/control


/**
 * From iOS
 */
/**
 * Will update the roaster object within the user object.
 *  @params
 *      :mode
 *          Valid options: start, pause, stop
 *      time=
 *          When the roasting started hh:mm:ss
 *      roastData=
 *          The name of the roast that is currently loaded on the
 *          roaster or if in mode 'live' the temperature, fan rate tuple
 *
 *  @return
 *
 */
router.post('/client', function (req, res) {

    //grab authentication token sent from the client that was attached to header
    var token = getToken(req.headers);

    if (token) {

        var decoded = jwt.decode(token, config.secret);

        //in url with name=value
        var mode = req.query.mode;
        var roast = req.query.roastData;

        console.log(mode);

        //check if required parameters have been passed
        if (roast == null || mode == null) {
            res.json({
                success: false,
                msg: "Make sure that you have passed both the mode you would like to set the roaster to along with the roast name that will run!"
            });
            return;
        }

        //is the mode valid?
        if (mode != "start" && mode != "pause" && mode != "stop" && mode != "live") {
            res.json({success: false, msg: "The mode passed is not valid!"});
            return;
        }

        //the -pending end is only needed for non-live operations
        if (mode != "live")
            mode += "-pending";

        User.update(
            {
                name: decoded.username
            },
            {
                roaster: {
                    roastingStatus: mode,
                    roastData: roast == null ? "same" : roast       //if in mode 'live' then will hold a tuple of data => 225,30 => temp,fan rate
                                                                    //otherwise, will hold the name of the currently running roast
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


                res.json({success: true, msg: 'Roast profile successfully set to ' + mode});
            }
        );
    }
    else {
        //error response
        return res.status(403).send({success: false, msg: 'No token provided.'});
    }
});

/**
 * State update from roaster
 */
router.post('/roaster', function (req, res) {

    //grab authentication token sent from the client that was attached to header
    var token = getToken(req.headers);

    if (token) {

        var decoded = jwt.decode(token, config.secret);

        //will be in url after ?
        var mode = req.query.mode;
        var roast = req.query.roastData;
        var time = req.query.time;

        console.log("mode => " + mode);
        console.log("roast => " + roast);
        console.log("time => " + time);

        //check if required parameters have been passed
        if (roast == null || mode == null) {
            res.json({
                success: false,
                msg: "Make sure that you have passed both the mode you would like to set the roaster to along with the roast name that will run!"
            });
            return;
        }

        //is the mode valid?
        if (mode != "start" && mode != "pause" && mode != "stop" && mode != "live") {
            res.json({success: false, msg: "The mode passed is not valid!"});
            return;
        }

        User.update(
            {
                name: decoded.username
            },
            {
                roaster: {
                    roastingStatus: mode,
                    roastStartTime: time,
                    roastData: roast            //if in mode 'live' then will hold a tuple of data => 225,30 => temp,fan rate
                                                //otherwise, will hold the name of the currently running roast
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
                    res.json({success: true, msg: 'Your roaster is now in ' + mode + ' mode'});
                }
            }
        );
    }
    else {
        //error response
        return res.status(403).send({success: false, msg: 'No token provided.'});
    }
});


//update on roast from either client or roaster via GET
router.get('/', function (req, res) {

    //grab authentication token sent from the client that was attached to header
    var token = getToken(req.headers);

    if (token) {

        var decoded = jwt.decode(token, config.secret);

        User.find(
            {
                name: decoded.username
            },
            function (err, user) {
                if (err){
                    res.json({success: false, error: err});
                    return;
                }


                console.log("user => " + user);

                if (user[0].roaster.roastingStatus == "start-pending") {
                    //get roast data
                    Roast.find(
                        {
                            name: user[0].roaster.roastData
                        },
                        function (err, roast) {
                            if (err)  res.json({success: false, error: err});

                            console.log("roast => " + roast);

                            res.json({success: true, roaster: user[0].roaster, roastData: roast[0]});
                        }
                    );

                    return;
                }

                res.json({success: true, roaster: user[0].roaster});
            }
        );
    }
    else {
        //error response
        return res.status(403).send({success: false, msg: 'No token provided.'});
    }
});

module.exports = router;
