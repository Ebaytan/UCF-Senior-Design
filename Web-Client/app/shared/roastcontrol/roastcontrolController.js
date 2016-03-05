'use strict';

angular.module('myApp.missioncontrol', [
        'myApp.missioncontrol.roasting'
    ])

    .controller('missionCtrl', function ($scope) {
        //determines current view of mission control
        $scope.currentState = "Initial";
        $scope.listType = "";   //Favorite, Recent, or Popular


        //Roasting State
        $scope.roasting = false;

        $scope.setnextState = function (mode) {
            switch (mode) {
                case 'Initial':
                    $scope.currentState = 'Initial';
                    $scope.listType = '';
                    $scope.roasting = false;
                    break;
                case 'Roasting':
                    $scope.currentState = 'Roasting';
                    $scope.listType = '';   //can cause errors???
                    $scope.roasting = true;
                    break;
                case 'Favorite':
                    $scope.currentState = 'list';
                    $scope.listType = 'Favorite';
                    $scope.roasting = false;
                    break;
                case 'Recent':
                    $scope.currentState = 'list';
                    $scope.listType = 'Recent';
                    $scope.roasting = false;
                    break;
                case 'Popular':
                    $scope.currentState = 'list';
                    $scope.listType = 'Popular';
                    $scope.roasting = false;
                    break;
                default:
                    $scope.currentState = 'Initial';
                    $scope.listType = '';   //can cause errors???
                    $scope.roasting = false;
                    break;
            }
        };

        $scope.toggleRoast = function () {
            $scope.roasting = !$scope.roasting;
            console.log('Roast has been toggled');
        };
    });
