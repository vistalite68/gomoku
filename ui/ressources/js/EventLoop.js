EventLoop = (function() {
	var playerTurn = 0;
	var canvasElem = $('#c')[0];
	return {
		init: function(canvas) {
			canvas.on('mouse:down', function(options) {

				var rect = canvasElem.getBoundingClientRect();
				var pos = {x: options.e.clientX - rect.left,
							y: options.e.clientY - rect.top};
				SocketModule.sendMove(CoordModule.getLocalPos(pos.x, pos.y), playerTurn);
				playerTurn++;
				playerTurn = playerTurn % 2;
			});
			canvas.on('mouse:move', function(options) {
				var rect = canvasElem.getBoundingClientRect();
				var pos = {x: options.e.clientX - rect.left,
							y: options.e.clientY - rect.top};
				BoardModule.setOverlayStone(CoordModule.getLocalPos(pos.x, pos.y));
			});
			canvas.on('mouse:out', function(options) {
				BoardModule.setDispOverlay(false);
			});
			canvas.on('mouse:over', function(options) {
				BoardModule.setDispOverlay(true);
			});
			$('#launch_new_game').click(function() {
				var p1 = $('#player_1_type').val();
				var p2 = $('#player_2_type').val();
				SocketModule.sendNewGame(p1, p2, 0);
			});
			$('#winner_modal_btn').click(function() {
				$('#winner_div').closeModal();
				$('#ask_new_game').openModal({dismissible:false});
			});
			$('#restart_game').click(function() {
				SocketModule.sendForceRestart();
			});
			$('#help').change(function() {
				BoardModule.setHelpDisplay($(this).is(':checked'));
			});
		}
	}
})();
