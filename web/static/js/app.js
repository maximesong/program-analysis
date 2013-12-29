(function() {
    angular.module("app", [])
	.controller("sourceCtrl", function($scope, $http, $location) {
	    $scope.dir = {
		'files':[],
		'dirs':[]
	    };
	    $http.get("/listdir").success(function (data) {
		$scope.current_dir = "/";
		$scope.dir_info = data;
	    });

	    $scope.refresh = function() {
	    }

	    $scope.$watch(function () {
		return $location.path();
	    }, function (newPath) {
		if (newPath == "" || newPath.endsWith("/")) {
		    $http.post("/listdir",
			       { "dir": newPath }).success(function (data) {
				   $scope.dir_info = data;
			       });
		} else {
		    console.log(newPath, "File");
		}
	    });
	});
})()
