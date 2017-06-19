<?php
/**
* @file
* User can history of Book proposals.
*/
function user_tbc_progress() {
// Return a string with html content of progress bar based on status
	drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/custom.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
	drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/custom_tbc.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
	drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/rejected.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
	drupal_add_js(drupal_get_path('module', 'user_dash') . '/js/progressBar.js', 'file');
	return '
			<div id="progressbar_container">
				<ul id="progressbar">
					<li id="node1"><p class="hidden">Submit Book Proposal</p></li>
					<li id="node2"><p class="hidden">Proposal Recieved, Waiting for approval</p></li>
					<li id="node3"><p class="hidden"><span id="approved">Proposal Approved, Start submitting codes</span><span id="rejected">Proposal Rejected</span></p></li>
					<li id="node4"><p class="hidden">Code received. Under review</p></li>
					<li id="node5"><p class="hidden">Code reviewed and Completed Book</p></li>
					<li id="node6"><p class="hidden">Book Completed</p></li>
				</ul>
			</div>
		</div>';
}

function user_tbc_manage_proposals() {
	$contents='';

	/* Get proposal db where user id is of current user.*/
	$book_rows = array();
	$result = db_query('SELECT textbook_companion_proposal.id,
							textbook_companion_preference.book_status,
							textbook_companion_proposal.new_proposal_status,
							textbook_companion_preference.book,
							textbook_companion_preference.edition,
							textbook_companion_preference.author,
							textbook_companion_preference.year
							FROM {textbook_companion_proposal} INNER JOIN {textbook_companion_preference} ON textbook_companion_preference.proposal_id=textbook_companion_proposal.id
							ORDER BY textbook_companion_proposal.id DESC;');

		$contents .= '<div id="selected_proposal" style="display:none;"><h4>Current Proposal</h4>';
		//   dpm(get_defined_vars());
		$contents .= user_tbc_progress();

		$contents .= '<h4>All Proposals</h4>';

		while ($book_data = $result->fetchObject()) {
			/* get preference */


			/*$preference_q = db_query("SELECT * FROM textbook_companion_preference WHERE proposal_id = %d AND approval_status = 1 LIMIT 1", $proposal_data->id);
					$preference_data = db_fetch_object($preference_q);*/
			$book_status = getBookStatus($book_data->book_status);
			$proposal_status = getProposalStatus($book_data->new_proposal_status);
			$book_rows[] = array(
				$book_data->id,
				$book_data->book . '<br> <em>by ' . $book_data->author . '</em>',
				$book_data->edition,
				$book_data->year,
				$proposal_status,
				$book_status,
				'<a href="#" onclick="selectProposal_TBC('.$book_data->new_proposal_status.','.$book_data->book_status.');">Select</a>',
			);
		}
		//$proposal_data = $proposal_q->fetchObject()
		/* check if there are any pending proposals */

		if(!$book_rows){
			//If no previous proposal are found.
			return $contents .= t('There are no proposal.');
		}

		//!$proposal_rows
		$book_header = array(
			'Proposal ID',
			'Book',
			'Edition',
			'Year',
			'Proposal Status',
			'Book Status',
			'Select Proposal',
		);
		
	$contents .= theme('table', array(
		'header' => $book_header,
		'rows' => $book_rows,
	));
	return $contents;
}

function getProposalStatus($proposal_status = 0){
	switch($proposal_status) {
		case 0:
			return 'Under Review';
		case 1:
			return 'Accepted';
		case 2:
			return 'Rejected';
		default:
			return 'Unknown';
	}
}

function getBookStatus($book_status = 0){
	switch ($book_status) {
		case 0:
			return 'In Progress';

		case 1:
			return 'Under Review';

		case 2:
			return 'Approved';

		case 3:
			return 'Completed';

		case 4:
			return 'Disabled';

		default:
			return 'Unknown';
	}
}
