<?php
/**
 * @file
 * User can history of lab migration proposals.
 */
function user_lab_migration_proposals_progress() {
drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/custom.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/custom_lab_migration.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/rejected.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
drupal_add_js(drupal_get_path('module', 'user_dash') . '/js/progressBar.js', 'file');

  return '
			<div id="progressbar_container">
				<ul id="progressbar">
					<li id="node1"><p class="hidden">Submit Proposal</p></li>
					<li id="node2"><p class="hidden">Proposal waiting for approval</p></li>
					<li id="node3"><p class="hidden"><span id="approved">approved waiting for Code Submission</span><span id="rejected">Proposal Rejected</span></p></li>
					<li id="node4"><p class="hidden">Solution received and approved. Lab is completed</p></li>
				</ul>
			</div>
		</div>';
}

function user_lab_migration_proposals() {
//For Admin
  $contents = '';
  //Get all proposal of current user(history) in table form.
  $proposal_rows = array();
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->orderBy('id', 'DESC');
  if (user_dash_user_is_admin($GLOBALS['user'])){
	$proposal_q = $query->execute();
	$contents .= '<div id="selected_proposal" style="display: none"><h4>Selected Proposal</h4>';
	$contents .= user_lab_migration_proposals_progress();
	$contents .= '<h4>All Proposals</h4>';

	while ($proposal_data = $proposal_q->fetchObject()) {
		$approval_status = getApprovalStatus($proposal_data->approval_status);
		$proposal_rows[] = array(
		$proposal_data->id,
		date('d-m-Y', $proposal_data->creation_date),
		$proposal_data->lab_title,
		$proposal_data->department,
		$approval_status,
		'<a href="#" onclick=selectProposal_LM('.$proposal_data->approval_status.')>Select</a>'
		);
	}
	if(!$proposal_rows){
		//If no previous proposal are found.
		return $contents .= t('There are no proposal.');
	}

	$proposal_header = array(
		'Proposal ID',
		'Date of Submission',
		'Title of the Lab',
		'Department',
		'Status',
		'Select Proposal'
	);
  }
  else {
//For Autheticated User
	$query->condition('uid', $GLOBALS['user']->uid);
	$proposal_q = $query->execute();
	$p = $proposal_q->fetchObject();
	if (!$p) {
		$contents .= t('You have not submitted any proposal yet. ') . l('Click here', 'lab_migration/proposal') . t(' to submit a proposal.');
		return $contents;
	}
	$contents .= '<div id="selected_proposal"><h4>Current Proposal</h4> Proposal ID: ' . $p->id;
	$contents .= user_lab_migration_proposals_progress();
	$contents .= '<h4>Previous Proposals</h4><script>selectProposal_LM('.$p->approval_status.');</script>';

	while ($proposal_data = $proposal_q->fetchObject()) {
		$approval_status = getApprovalStatus($proposal_data->approval_status);
		$proposal_rows[] = array(
		$proposal_data->id,
		date('d-m-Y', $proposal_data->creation_date),
		$proposal_data->lab_title,
		$proposal_data->department,
		$approval_status,
		);
	}
	if(!$proposal_rows){
		//If no previous proposal are found.
		return $contents .= t('There are no previous proposal.');
	}

	$proposal_header = array(
		'Proposal ID',
		'Date of Submission',
		'Title of the Lab',
		'Department',
		'Status',
	);
  }
  $contents .= theme('table', array(
      'header' => $proposal_header,
      'rows' => $proposal_rows,
    ));
  return $contents;
}

function user_dash_user_is_admin() {
  //This function checks if user is admin or not.
  global $user;
  if (is_array($user->roles) && in_array('administrator', array_values($user->roles))) {
    //If admin role is found in user's roles return TRUE.
    //			print "TRUE";
    return TRUE;
  }
  //If admin role is not found return FALSE.
  //		print "FALSE";
  return FALSE;
}

function getApprovalStatus($approval_status = 0){
	switch ($approval_status) {
      case 0:
        return 'Pending';

      case 1:
        return 'Approved';

      case 2:
        return 'Dis-approved';

      case 3:
        return 'Solved';

      default:
        return 'Unknown';
    }
}
