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
    //target temperature
    RoastingData: {
        required: true,
        type: Number
    }
});

module.exports = mongoose.model('Roast', RoastSchema);

