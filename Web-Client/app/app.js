'use strict';

//Check if 'yourTokenKey' exists if so do a get request to get the user's name and roaster state

angular.module('myApp', [
    'ngRoute',
    'myApp.view1',
    'myApp.view2',
    'myApp.login',
    'myApp.signup',
    "myApp.browse",
    'myApp.missioncontrol',
    'myApp.version',
    'ngAnimate',
    'ngSanitize',
    'mgcrea.ngStrap',
    'jcs-autoValidate',
    'chart.js',
    'myApp.documentation'
])
    .controller('general', function ($rootScope, AuthService) {
        $rootScope.Authenticated = false;
        $rootScope.AuthenticatedUser = '';

        //not stored in var???? issue???
        AuthService.loadUserCredentials();
    })
    .config(['$routeProvider', function ($routeProvider) {
        $routeProvider.otherwise({redirectTo: '/'});
    }]);