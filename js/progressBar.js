function selectProposal_LM(proposal_status = 0){
	//Fills the progressBar with the proposal_status.
	console.log("selectProposal_LM called: " + proposal_status);
	document.getElementById('selected_proposal').style.display = 'initial';
	resetProgressBar(4);
	makeNodeActive('node1');
	if (proposal_status >= 0 && proposal_status < 4) {
		makeNodeActive('node2');
	}
	if (proposal_status >= 1 && proposal_status < 4) {
		makeNodeActive('node3');
	}
	if (proposal_status == 2) {
		makeNodeActive('node3');
		makeNodeRejected('approved','rejected');
// 		drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/rejected.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
	}
	if (proposal_status == 3) {
		makeNodeActive('node4');
	}
}

function selectProposal_TBC(proposal_status=0, book_status=0){
	//Fills the progressBar with proposal_status and book_status.
	console.log("selectProposal_TBC called: " + proposal_status + ", " + book_status);
	document.getElementById('selected_proposal').style.display = 'initial';
	resetProgressBar(6);
	makeNodeActive('node1');
	if (proposal_status >= 0 && proposal_status < 2) {
		//Proposal Recieved
		makeNodeActive('node2');
	}
	if (proposal_status = 1){
		makeNodeActive('node3');
		//Proposal approved
		if(book_status > 0 && book_status < 5){
			//Code Recieved
			makeNodeActive('node4');
		}
		if(book_status > 1 && book_status < 5){
			//Code reviewed
			makeNodeActive('node5');
		}
		if(book_status > 2 && book_status < 5){
			//Book Completed
			makeNodeActive('node5');
// 			makeNodeActive('node6');
		}
	}
	else if (proposal_status = 2){
		//Proposal rejected
// 		drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/rejected.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
		makeNodeRejected('approved','rejected');
	}
}

function resetProgressBar(nodes_n){
	//Helper function to reset the progress bar.
	for(var i=1; i <= nodes_n; i++){
		document.getElementById('node' + i).classList.remove('active');
		document.getElementById('node' + i).classList.remove('active_rej');
	}
	document.getElementById('approved').style.display = 'block';
	document.getElementById('rejected').style.display = 'none';
}

function makeNodeActive(id){
	//Helper function to make node active.
	document.getElementById(id).classList.add('active');
}

function makeNodeRejected(id_accepted, id_rejected){
	//Helper function to make node rejected.
	document.getElementById(id_accepted).style.display = 'none';
	document.getElementById(id_rejected).style.display = 'block';
	document.getElementById(id_rejected).parentNode.parentNode.classList.add('active_rej');
}
