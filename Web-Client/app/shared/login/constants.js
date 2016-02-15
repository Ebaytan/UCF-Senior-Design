angular.module('myApp.login')

    .constant('AUTH_EVENTS', {
        notAuthenticated: 'auth-not-authenticated'
    })

    .constant('API_ENDPOINT', {
        url: 'http://127.0.0.1:9000/api'
        //  For a simulator use:
        //url: 'http://127.0.0.1:8080/api'
    });
