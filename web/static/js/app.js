(function() {
  angular.module("app", [])
    .controller("sourceCtrl", function($scope, $http, $location) {
      var POST_URL = "/serverdata";
      $scope.dir = {
	'files':[],
	'dirs':[]
      };
      if (!$location.path()) {
        $location.path("/");
      }

      $scope.refresh = function() {
      };

      $scope.isCurrentPathADir = function() {
        return $scope.currentPath.endsWith("/");
      };

      $scope.tabSelected = 'Source';

      $scope.onTabSelected = function(tab) {
        $scope.tabSelected = tab;
      };

      $scope.isTabSelected = function(tab) {
        return $scope.tabSelected === tab;
      };

      $scope.is_root_dir = function() {
	return $scope.dir_info && $scope.dir_info.base === $scope.dir_info.parent;
      };

      $scope.$watch(
        function () {
	  return $location.path();
        },
        function (newPath) {
          var currentDir;
          $scope.currentPath = newPath;
	  if ($scope.isCurrentPathADir()) {
            currentDir = newPath;
	    $scope.sourceCode = "";
	  } else {
            currentDir = newPath.replace(/\/[^\/]*$/, '/');
          }
          $http.post(POST_URL,
		     {
                       type: "dir",
                       dir: currentDir
                     }
                    ).success(function (data) {
		      $scope.dir_info = data;
		    });
          if (!$scope.isCurrentPathADir()) {
            $http.post(POST_URL,
		       { 
                         type: "file",
                         "filename": newPath
                       }
                      ).success(function (data) {
		        $scope.sourceCode = data;
		      });
            $http.post(POST_URL,
		       { 
                         type: "cfg",
                         "filename": newPath
                       }
                      ).success(function (data) {
		        $scope.cfgJSON = data;
		      });
            $http.post(POST_URL,
		       { 
                         type: "cg",
                         "filename": newPath
                       }
                      ).success(function (data) {
		        $scope.cgJSON = data;
		      });
	  }
        });
    });
})();
