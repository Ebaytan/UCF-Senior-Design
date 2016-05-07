'use strict';

angular.module('myApp.missioncontrol.roasting', [])
    .controller('TimerCtrl', function ($scope, $timeout) {

        //TODO: Values will normally be pulled from db (create service to do so)
        $scope.eta = 970;
        $scope.elasped = 0;

        $scope.etaView = '';
        $scope.elaspedView = '';

        $scope.counter = 100;
        var stopped;

        $scope.count = function () {
            stopped = $timeout(function () {
                console.log($scope.elasped);
                console.log($scope.eta);
                //$scope.counter--;
                $scope.eta--;
                $scope.elasped++;

                if ($scope.eta < 0) {
                    return;
                }

                $scope.redraw();

                $scope.count();
            }, 1000);
        };


        $scope.stop = function () {
            $timeout.cancel(stopped);

        };

        ////compose new string to print
        $scope.redraw = function () {
            var sec = $scope.eta % 60;
            var min = Math.floor($scope.eta / 60);

            if (sec < 10) {
                $scope.etaView = min + ':0' + sec;
            }
            else {
                $scope.etaView = min + ':' + sec;
            }

            sec = $scope.elasped % 60;
            min = Math.floor($scope.elasped / 60);
            if (sec < 10) {
                $scope.elaspedView = min + ':0' + sec;
            }
            else {
                $scope.elaspedView = min + ':' + sec;
            }

        };

        $scope.count();

        //Will normally be pulled from db (Create Service to do that)
        //$scope.eta = 1000;
        //$scope.elasped = 0;
        //
        //$scope.increment = function () {
        //    $scope.eta--;
        //    $scope.etaView = '';
        //    $scope.elasped++;
        //    $scope.elaspedView = '';
        //
        //    if ($scope.eta < 0) {
        //        return;
        //    }
        //
        //    console.log($scope.eta);
        //    console.log($scope.elasped);
        //
        //    var timer = $timeout($scope.increment(), 10000);
        //
        //    $scope.redraw();
        //
        //};
        //
        //$scope.start = function () {
        //    $scope.increment();
        //};
        //
        //$scope.stop = function () {
        //    $timeout.cancel(timer);
        //};
        ////compose new string to print
        //$scope.redraw = function () {
        //    var sec = $scope.eta % 60;
        //    var min = $scope.eta / 60;
        //
        //    if (sec < 10) {
        //        $scope.etaView = min + ':0' + sec;
        //    }
        //    else {
        //        $scope.etaView = min + ':' + sec;
        //    }
        //
        //    $scope.etaView = min + ':' + sec;
        //
        //    sec = $scope.elasped % 60;
        //    min = $scope.elasped / 60;
        //    if (sec < 10) {
        //        $scope.elaspedView = min + ':0' + sec;
        //    }
        //    else {
        //        $scope.elaspedView = min + ':' + sec;
        //    }
        //
        //};
        //
        ////start counter automatically
        //$scope.start();

    });