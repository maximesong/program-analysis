(function() {
  angular.module("app", [])
    .controller("sourceCtrl", function($scope, $http, $location) {
      $scope.dir = {
	'files':[],
	'dirs':[]
      };
      if (!$location.path()) {
        $location.path("/");
      }

      $scope.refresh = function() {
      };

      $scope.is_root_dir = function() {
	return $scope.dir_info && $scope.dir_info.base === $scope.dir_info.parent;
      };

      $scope.$watch(
        function () {
	  return $location.path();
        },
        function (newPath) {
          $scope.current_dir = newPath;
	  if (newPath == "" || newPath.endsWith("/")) {
	    $http.post("/listdir",
		       { "dir": newPath }
                      ).success(function (data) {
		        $scope.dir_info = data;
		      });
	  } else {
	    console.log(newPath, "File");
	  }
        });
    });
})();
