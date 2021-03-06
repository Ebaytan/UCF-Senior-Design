var mongoose = require('mongoose');
var Schema = mongoose.Schema;
var bcrypt = require('bcrypt');


// set up a mongoose model
var UserSchema = new Schema({
    username: {
        type: String,
        required: true,
        unique: true

    },
    password: {
        type: String,
        required: true
    },
    email: {
        type: String,
        required: true
    },
    //Will initially be null until a roasting profile is rated
    rating: {
        avgRating: Number,          //total/number of ratings
        totalAccRating: Number,     //Total accumulated ratings
        numberofRatings: Number     //Total number of ratings
    },

    roaster: {
        //unique identifier for each roaster (Assuming each user has only one roaster)
        roasterUID: {
            type: String,
            required: true
        },
        roastingStatus: String,     //current mode of operation
        roastStartTime: String,
        roastData: String           //if in mode 'live' then will hold a tuple of data => 225,30 => temp,fan rate
                                    //otherwise, will hold the name of the currently running roast
    },

    roastingprofiles: []
});

UserSchema.pre('save', function (next) {
    var user = this;
    if (this.isModified('password') || this.isNew) {
        bcrypt.genSalt(10, function (err, salt) {
            if (err) {
                return next(err);
            }
            bcrypt.hash(user.password, salt, function (err, hash) {
                if (err) {
                    return next(err);
                }
                user.password = hash;
                next();
            });
        });
    } else {
        return next();
    }
});

UserSchema.methods.comparePassword = function (passw, cb) {
    bcrypt.compare(passw, this.password, function (err, isMatch) {
        if (err) {
            return cb(err);
        }
        cb(null, isMatch);
    });
};

module.exports = mongoose.model('User', UserSchema);
