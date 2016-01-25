'use strict';

angular.module('myApp.browse', ['ngRoute'])

    .config(['$routeProvider', function($routeProvider) {
        $routeProvider.when('/browse', {
            templateUrl: 'browse/browse.html',
            controller: 'browseCtrl'
        });
    }])

    .controller('browseCtrl', [function() {

    }]);