'use strict';

angular.module('myApp.signup', ['ngRoute'])

    .config(['$routeProvider', function($routeProvider) {
        $routeProvider.when('/signup', {
            templateUrl: 'views/signup/signup.html',
            controller: 'signupCtrl'
        });
    }])

    .controller('signupCtrl', [function() {

    }]);