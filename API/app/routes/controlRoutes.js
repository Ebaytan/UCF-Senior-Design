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
*       roastname=
 *          The name of the roast that is currently loaded on the roaster
 *
 *  @return
 *
 */
router.post('/client/:mode', function (req, res) {

    //grab authentication token sent from the client that was attached to header
    var token = getToken(req.headers);

    if (token) {

        var decoded = jwt.decode(token, config.secret);
        var mode = req.params.mode;
        var roast = req.query.roastName;
        var time = req.query.time;

        console.log(mode);

        //check if required parameters have been passed
        if(roast == null || mode == null){
            res.json({success: false, msg: "Make sure that you have passed both the mode you would like to set the roaster to along with the roast name that will run!"});
            return;
        }

        //is the mode valid?
        if (mode != "start" && mode != "pause" && mode != "stop") {
            res.json({success: false, msg: "The mode passed is not valid!"});
            return;
        }

        User.update(
            {
                name: decoded.username
            },
            {
                roaster: {
                    roastingStatus: mode + '-pending', //
                    roastStartTime: time,
                    roastProfileID: roast
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
                    res.json({success: true, msg: 'Roast profile successfully set to ' + mode + '-pending!'});
                }
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
router.post('/roaster/:mode', function (req, res) {

    //grab authentication token sent from the client that was attached to header
    var token = getToken(req.headers);

    if (token) {

        var decoded = jwt.decode(token, config.secret);
        var mode = req.params.mode;
        var roast = req.query.roastName;
        var time = req.query.time;

        console.log(mode);

        //check if required parameters have been passed
        if(roast == null || mode == null){
            res.json({success: false, msg: "Make sure that you have passed both the mode you would like to set the roaster to along with the roast name that will run!"});
            return;
        }

        //is the mode valid?
        if (mode != "start" && mode != "pause" && mode != "stop") {
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
                    roastProfileID: roast
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
                    res.json({success: true, msg: 'Your roaster is now in ' + mode + 'mode'});
                }
            }
        );

    }

    else {
        //error response
        return res.status(403).send({success: false, msg: 'No token provided.'});
    }

});



module.exports = router;
