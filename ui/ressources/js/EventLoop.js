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
			$('#launch_new_game').click(function() {
				SocketModule.sendNewGame();
			});
			$('#winner_modal_btn').click(function() {
				$('#winner_div').closeModal();
				$('#ask_new_game').openModal({dismissible:false});
			});
		}
	}
})();
