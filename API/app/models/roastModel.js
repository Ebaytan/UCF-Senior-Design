var mongoose = require('mongoose');
var Schema = mongoose.Schema;

var RoastSchema = new Schema({
    name: {
        type: String,
        required: true,
        unique: true
    },
    roastType: {
        type: String,
        required: true
    },
    beanType: {
        type: String,
        required: true
    },
    //Will initially be null until a roasting profile is rated
    rating: {
        required: true,
        type: Number
    },
    creatorID: {
        required: true,
        type: String
    },
    /**
     * Format of data:
     *      time_0,temp_0,air_0;time_1,temp_1,air_1;time_2,temp_2,air_2;...
     * Notes:
     *      1) In order to show that the roast is to be finished
     *         the last tuple must have a '0' for the temp and air
     *         values.  For example 404,532,32;532,0,0
     *          2) Temperature is assumed to be in degrees Fahrenheit
     *          3) Air rate is a percentage of the max fan rate possible, so
     *         if you would like to the fan to run at 50% capacity
     *         pass 50 into the air value of the tuple.
     */
    RoastingData: {
        required: true,
        type: String
    }

});

module.exports = mongoose.model('Roast', RoastSchema);