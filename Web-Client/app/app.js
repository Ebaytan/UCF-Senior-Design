'use strict';

// Declare app level module which depends on views, and components
angular.module('myApp', [
        'ngRoute',
        'myApp.view1',
        'myApp.view2',
        'myApp.login',
        'myApp.signup',
        'myApp.version',
        'ngAnimate',
        'ngSanitize',
        'mgcrea.ngStrap',
        'jcs-autoValidate'

    ])
    .config(['$routeProvider', function ($routeProvider) {
        $routeProvider.otherwise({redirectTo: '/'});
    }])
    .controller('general', function ($scope) {

    });
