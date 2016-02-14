'use strict';

angular.module('myApp.browse', [
        'ngRoute',
        'myApp.browse.roastprofile',
        'myApp.browse.browselist'

    ])

    .config(['$routeProvider', function ($routeProvider) {
        $routeProvider.when('/browse', {
            templateUrl: './views/browse/browse.html',
        });
        $routeProvider.when('/browse/lightroasts', {
            templateUrl: './views/browse/browselist/browselist.html'
        });
        $routeProvider.when('/browse/view/:id', {
            templateUrl: './views/browse/roastprofile/roastprofile.html',
            controller: 'roastprofileCtrl'
        });
    }])

    /*
    TODO: refactor controller into roast profile
     */
    .controller('roastprofileCtrl', ['$scope', '$timeout', function ($scope, $timeout) {
        /*
         On page load do a get request using
         parameter id and fill out page
         */
        $scope.series = ['Air vs Time', 'Temperature vs Time'];
        $scope.data = [
            [65, 59, 80, 81, 56, 55, 40],
            [28, 48, 40, 19, 86, 27, 90]
        ];
        //
        $scope.labels = ["January", "February", "March", "April", "May", "June", "July"];
        $scope.onClick = function (points, evt) {
            console.log(points, evt);
        };


    }]);





