'use strict';

angular.module('myApp.view2', ['ngRoute'])

    .config(['$routeProvider', function ($routeProvider) {
        $routeProvider.when('/aboutus', {
            templateUrl: 'views/aboutus/aboutus.html',
            controller: 'View2Ctrl'
        });
    }])

    .controller('View2Ctrl', [function () {

    }]);