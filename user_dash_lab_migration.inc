<?php

/**
 * @file
 * User can view history of lab migration proposals.
 */
function user_lab_migration_proposals() {
  if (user_access('access user_dash')) {

    //Empty string for filling content in it.
    $contents = '';
    //Empty array for getting table rows.
    $proposal_rows = array();
    //Select the table in database.
    $query = db_select('lab_migration_proposal');
    //Select fields in table (here all fields).
    $query->fields('lab_migration_proposal');
    //Order the results by id in descending order.
    $query->orderBy('id', 'DESC');
    //Get rows only with current user's uid present.
    $query->condition('uid', $GLOBALS['user']->uid);
    //Execute Query and save rows/results in $proposal_q
    $proposal_q = $query->execute();
    //Now fetch first row from results.
    $p = $proposal_q->fetchObject();

    //Check for if data is present is fetched or not.
    if (!$p) {
      //If no data is fetched than it means that no proposal has been submitted by that user.
      $contents .= t('You have not submitted any proposal yet. ') . l('Click here', 'lab_migration/proposal') . t(' to submit a proposal.');
      return $contents;
    }

    //If data is fetched then display the propsal id.
    $contents .= '<div id="selected_proposal"><h4>Current Proposal</h4> Proposal ID: ' . $p->id;
    //Get the progressBar and append it to contents.
    $contents .= user_lab_migration_proposals_progress();
    //Fill the progress bar with current proposal status.
    $contents .= '<h4>Previous Proposals</h4><script>selectProposal_LM(' . $p->approval_status . ');</script>';

    //Fetch all the remaining rows from results('$propsal_q') and save it in form table.
    while ($proposal_data = $proposal_q->fetchObject()) {
      //Get approval Status of the proposal
      $approval_status = getApprovalStatus($proposal_data->approval_status);
      $proposal_rows[] = array(
        //Id, Creation Date, Lab Title, Department, Status
        $proposal_data->id,
        date('d-m-Y', $proposal_data->creation_date),
        $proposal_data->lab_title,
        $proposal_data->department,
        $approval_status,
      );
    }

    //If no row is fetched from remaining results then return 'No previous proposals'.
    if (!$proposal_rows) {
      //If no previous proposal are found.
      return $contents .= t('There are no previous proposal.');
    }
    //Headers for table to be rendered on page.
    $proposal_header = array(
      'Proposal ID',
      'Date of Submission',
      'Title of the Lab',
      'Department',
      'Status',
    );

    //Create a table in form html and append it to contents.
    $contents .= theme('table', array(
        'header' => $proposal_header,
        'rows' => $proposal_rows,
      ));

    //return contents to render on page.
    return $contents;
  }
}

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

function getApprovalStatus($approval_status = 0) {
  //Returns the status(in string) of approval status( fetched from db).
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

