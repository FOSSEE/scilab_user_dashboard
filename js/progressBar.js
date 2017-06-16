function selectProposal_LM(proposal_status = 0){

// 	if ($approval_status >= 0 && $approval_status < 4) {
// 		$li[1] = 'active';
// 	}
// 	if ($approval_status >= 1 && $approval_status < 4) {
// 		$li[2] = 'active';
// 	}
// 	if ($approval_status == 2) {
// 		$li[2] = 'active';
// 		drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/rejected.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
// 	}
// 	if ($approval_status == 3) {
// 		$li[3] = 'active';
// 	}
// 	else $li[3] = '';

	resetProgressBar(4);
	makeNodeActive('node1');
}

function resetProgressBar(nodes_n){
	for(var i=1; i <= nodes_n; i++){
		document.getElementById('node' + i).classList.remove('active');
		document.getElementById('node' + i + ':before').classList.remove('active');
		document.getElementById('node' + i + ':after').classList.remove('active');
	}
	document.getElementById('approved').style.display = 'block';
	document.getElementById('rejected').style.display = 'none';
}

function makeNodeActive(id){
	document.getElementById(id).classList.add('active');
	document.getElementById(id).classList.add('active:before');
	document.getElementById(id).classList.add('active:after');
}

function makeNodeRejected(id_accepted, id_rejected){
	document.getElementById(id_accepted).style.display = 'none';
	document.getElementById(id_rejected).style.display = 'block';
}
