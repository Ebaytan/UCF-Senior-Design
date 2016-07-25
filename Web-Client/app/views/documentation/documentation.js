'use strict';

angular.module('myApp.documentation', ['ngRoute'])

    .config(['$routeProvider', function ($routeProvider) {
        $routeProvider.when('/documents', {
            templateUrl: 'views/documentation/documentation.html',
            controller: 'documentationCtrl'
        });
    }])

    .controller('documentationCtrl', [function () {

    }]);