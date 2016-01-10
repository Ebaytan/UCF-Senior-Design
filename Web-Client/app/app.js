'use strict';

// Declare app level module which depends on views, and components
angular.module('myApp', [
        'ngRoute',
        'myApp.view1',
        'myApp.view2',
        'myApp.login',
        'myApp.version',
        'ngAnimate',
        'ngSanitize',
        'mgcrea.ngStrap',
        'jcs-autoValidate'

    ])
    .config(['$routeProvider', function ($routeProvider) {
        $routeProvider.otherwise({redirectTo: '/view1'});
    }])
    .config(function ($modalProvider) {
        angular.extend($modalProvider.defaults, {
            html: true
        });
    })
    .controller('Modals', function ($scope, $modal) {
        var loginModal = $modal({
            scope: $scope,
            templateUrl: '../app/partials/login.tpl.html',
            show: false
        });
        var signupModal = $modal({
            scope: $scope,
            templateUrl: '../app/partials/signup.tpl.html',
            show: false
        });

        //show login modal
        $scope.showLoginModal = function () {
            loginModal.$promise.then(loginModal.show);
        };

        //show sign up modal
        $scope.showSignupModal = function () {
            signupModal.$promise.then(signupModal.show);
        };

        /*$scope.modal = {title: 'Title', content: 'Hello Modal<br />This is a multiline message!'};*/

        // Controller usage example
        //
        /*        function MyModalController($scope) {
         $scope.title = 'Some Title';
         $scope.content = 'Hello Modal<br />This is a multiline message from a controller!';
         }
         MyModalController.$inject = ['$scope'];
         var myModal = $modal({controller: MyModalController, templateUrl: '../app/partials/login.tpl.html', show: false});
         $scope.showModal = function() {
         myModal.$promise.then(myModal.show);
         };
         $scope.hideModal = function() {
         myModal.$promise.then(myModal.hide);
         };

         $scope.modal = {title: 'Title', content: 'Hello Modal<br />This is a multiline message!'};*/

    });
