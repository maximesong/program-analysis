(function() {
  // endsWith is not available for chrome
  if (typeof String.prototype.endsWith !== 'function') {
    String.prototype.endsWith = function(suffix) {
      return this.indexOf(suffix, this.length - suffix.length) !== -1;
    };
  }
  angular.module("app", [])
    .controller("sourceCtrl", function($scope, $http, $location) {
      var POST_URL = "/serverdata";
      $scope.DEBUG = false;
      $scope.dir = {
	'files':[],
	'dirs':[]
      };
      if (!$location.path()) {
        $location.path("/");
      }

      $scope.debug = function() {
        $scope.DEBUG = !$scope.DEBUG;
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
		        var cfgList = [];
		        for (var i = 0; i < data.funclist.length; i++) {
		          var oneFuncJSON = data.funclist[i];
		          var oneCFG = {
		            id : "",
		            blocks : [],
		          }
		          oneCFG.id = oneFuncJSON.id;
		          for (var j = 0; j < oneFuncJSON.blocks.length; j++) {
		            var oneBlock = oneFuncJSON.blocks[j];
		            var blockInfo = {
		              elements : "",
		              block_id : 0,
		              succ_id_list : [],
		              pred_id_list : []
		            }
		            blockInfo.block_id = oneBlock.block_id;
		            blockInfo.pred_id_list = oneBlock.pred_id_list;
		            blockInfo.succ_id_list = oneBlock.succ_id_list;
		            var simpleElements = [];
		            for (var m = 0; m < oneBlock.elements.length; m++) {
		              var oneElement = oneBlock.elements[m];
		              var bool = true;
		              for (var n = 0; n < simpleElements.length; n++) {
		                if (oneElement.start_line == simpleElements[n].start_line) {
		                  bool = false;
		                }
		              }
		              if (bool == true) {
		                simpleElements.push(oneElement);
		              }
		            }
		            for (var k = 0; k < simpleElements.length-1; k++) {
		              blockInfo.elements += simpleElements[k].start_line + ":";
		              blockInfo.elements += simpleElements[k].code;
		              blockInfo.elements += "\n";
		            }
		            if (simpleElements.length > 0) {
		              blockInfo.elements += simpleElements[k].start_line + ":";
		              blockInfo.elements += simpleElements[simpleElements.length-1].code;
		            }
		            if (blockInfo.block_id == oneFuncJSON.entry_id) {
		              blockInfo.elements = "ENTRY";
		            }
		            if (blockInfo.block_id == oneFuncJSON.exit_id) {
		              blockInfo.elements = "EXIT";
		            }
		            oneCFG.blocks.push(blockInfo);
		          }
		          cfgList.push(oneCFG);
		        }
		        $scope.cfgList = cfgList;
		      });
            $http.post(POST_URL,
		       { 
                         type: "cg",
                         "filename": newPath
                       }
                      ).success(function (data) {
		        $scope.cgJSON = data;
//                        data = JSON.stringify(data).replace(/>/g, "").replace(/</g, "");
                        console.log(data);
		        var graph = new Springy.Graph();
                        graph.loadJSON(data);
                        $("#springydemo").remove();
                        $("#canvas-container").append('<canvas id="springydemo" width="800", height="480"/>');
                        var canvas = document.getElementById('springydemo');
//                        var canvas = $('#springydemo');
                        console.log(canvas);
                        var context = canvas.getContext('2d');
                        context.clearRect(0, 0, canvas.width, canvas.height);
                        var springy = $('#springydemo').springy({
                          graph: graph
                        });
                      });
	  }
        });
    });
})();
