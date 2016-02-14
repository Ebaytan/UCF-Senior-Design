'use strict';

angular.module('myApp.browse.roastprofile', ['ngRoute'])

    .config(['$routeProvider', function ($routeProvider) {
        $routeProvider.when('/', {
            templateUrl: 'views/home/home.html',
            controller: 'View1Ctrl'
        });
    }])

    .controller('View1Ctrl', [function () {

    }]);