(function() {
    angular.module("app", [])
	.controller("sourceCtrl", function($scope, $http) {
	    $scope.dir = {
		'files':[],
		'dirs':[]
	    };
	    $http.get("/listdir").success(function (data) {
		$scope.dir = data;
	    });

	    $scope.refresh = function() {
	    }
	});
})()
