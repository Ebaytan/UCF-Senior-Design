angular.module('myApp.login', [])
    //$rootScope,
    .controller('LoginCtrl', function ($rootScope, $scope, AuthService, $window) {
        $scope.user = {
            username: '',
            password: ''
        };

        $scope.login = function () {
            AuthService.login($scope.user).then(function (msg) {
                $rootScope.Authenticated = true;
                console.log('User ' + $rootScope.AuthenticatedUser + ' has signed in!');
                console.log('logged in successfully');
            }, function (errMsg) {
                console.log(errMsg);
            });
        };
    })

    .controller('InsideCtrl', function ($scope, AuthService, API_ENDPOINT, $http, $state) {
        $scope.destroySession = function () {
            AuthService.logout();
        };

        $scope.getInfo = function () {
            $http.get(API_ENDPOINT.url + '/memberinfo').then(function (result) {
                $scope.memberinfo = result.data.msg;
            });
        };

        $scope.logout = function () {
            AuthService.logout();
        };
    })

    .controller('AppCtrl', function ($scope, $state, $ionicPopup, AuthService, AUTH_EVENTS) {
        $scope.$on(AUTH_EVENTS.notAuthenticated, function (event) {
            AuthService.logout();
        });
    });
