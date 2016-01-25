var mongoose = require('mongoose');

// user model
/*
    email
        - will be used to login with password
    password
        - will be hashed prior to storing with bcrypt
    rating
        - will be the overall average,  create object??
        - average, total number of comments, total
          accumulated rating will be used to recalculate
          each time.
    roasterid
        - the unique identifier assigned to each individual
          roaster produced.
    roastingprofiles
        - the ids of roasting profiles submitted by ced user
        - push into the array each time a new one is added into
          the roasting profiles collections
 */
var userSchema = new mongoose.Schema({
    name: String,
    email: String,
    password: String,
    rating: {
        avgRating: String,
        totalAccRating: Number,
        numberofRatings: Number
    },
    roasterUID: String,
    roastingprofiles: [],
    created_at: { type: Date, default: Date.now },
});

module.exports = mongoose.model('User', userSchema);
