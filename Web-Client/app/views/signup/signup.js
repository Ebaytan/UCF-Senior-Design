'use strict';

angular.module('myApp.signup', ['ngRoute'])

    .config(['$routeProvider', function($routeProvider) {
        $routeProvider.when('/signup', {
            templateUrl: 'views/signup/signup.html',
            controller: 'RegisterCtrl'
        });
    }])

    .controller('RegisterCtrl',function ($scope, AuthService, $window) {
        $scope.user = {
            username: '',
            password: '',
            email: '',
            roasterUID: ''
        };

        $scope.signup = function () {
            AuthService.register($scope.user).then(function (msg) {
                //TODO: create login own page
                $window.location.href = '#/';
            }, function (errMsg) {
                console.log(errMsg);
            });
        };
    });