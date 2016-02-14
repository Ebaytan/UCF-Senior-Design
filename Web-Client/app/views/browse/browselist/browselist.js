'use strict';

angular.module('myApp.browse.browselist', ['ngRoute'])

    .config(['$routeProvider', function($routeProvider) {
        $routeProvider.when('/browselist/:listtype', {
            templateUrl: './views/browselist/browselist.html',
            controller: 'browselistCtrl'
        });
    }])

    .controller('browselistCtrl', [function() {

    }]);