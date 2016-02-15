angular.module('myApp.login')

    .service('AuthService', function($q, $http, API_ENDPOINT, $rootScope) {
        var LOCAL_TOKEN_KEY = 'yourTokenKey';
        var LOCAL_USERNAME = 'defaultUsername';
        var isAuthenticated = false;
        var authToken;

        function loadUserCredentials() {
            var token = window.localStorage.getItem(LOCAL_TOKEN_KEY);
            var user = window.localStorage.getItem(LOCAL_USERNAME);
            //make sure we have a token and the username stored locally
            if (token && user) {
                useCredentials(token, user);
            }
        }

        var loadUser = function(){
            loadUserCredentials();
        }

        function storeUserCredentials(token,username) {
            window.localStorage.setItem(LOCAL_TOKEN_KEY, token);
            console.log('Store credentials as ' + username);
            window.localStorage.setItem(LOCAL_USERNAME, username);
            useCredentials(token,username);
        }

        function useCredentials(token, username) {
            isAuthenticated = true;
            $rootScope.Authenticated = true;
            console.log("useCredentials function's username" + username);
            $rootScope.AuthenticatedUser = username;
            authToken = token;

            // Set the token as header for your requests!
            $http.defaults.headers.common.Authorization = authToken;
        }

        function destroyUserCredentials() {
            authToken = undefined;
            isAuthenticated = false;
            $http.defaults.headers.common.Authorization = undefined;
            window.localStorage.removeItem(LOCAL_TOKEN_KEY);
            window.localStorage.removeItem(LOCAL_USERNAME);
        }

        var register = function(user) {
            return $q(function(resolve, reject) {
                $http.post(API_ENDPOINT.url + '/user/signup', user).then(function(result) {
                    if (result.data.success) {
                        console.log(result.data.msg);
                        resolve(result.data.msg);
                    } else {
                        console.log(result.data.msg);
                        reject(result.data.msg);
                    }
                });
            });
        };

        var login = function(user) {
            return $q(function(resolve, reject) {
                $http.post(API_ENDPOINT.url + '/user/authenticate', user).then(function(result) {
                    if (result.data.success) {
                        storeUserCredentials(result.data.token, user.username);
                        resolve(result.data.msg);
                    } else {
                        reject(result.data.msg);
                    }
                });
            });
        };

        var logout = function() {
            destroyUserCredentials();
        };

        loadUserCredentials();

        return {
            login: login,
            register: register,
            logout: logout,
            isAuthenticated: function() {return isAuthenticated;},
            loadUserCredentials: loadUser
        };
    });

    //.factory('AuthInterceptor', function ($rootScope, $q, AUTH_EVENTS) {
    //    return {
    //        responseError: function (response) {
    //            $rootScope.$broadcast({
    //                401: AUTH_EVENTS.notAuthenticated,
    //            }[response.status], response);
    //            return $q.reject(response);
    //        }
    //    };
    //})
    //
    //.config(function ($httpProvider) {
    //    $httpProvider.interceptors.push('AuthInterceptor');
    //});