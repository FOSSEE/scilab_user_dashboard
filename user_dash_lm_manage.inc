<?php

/**
 * @file
 * User can view and manage all lab migration proposals.
 */
function user_lab_migration_manage_proposals() {
  if (user_access('access user_dash_lm manage')) {
    //Create a empty string for storing html contents.
    $contents = '';
    //Create a empty array for storing table rows(html).
    $proposal_rows = array();
    //Select table in db.
    $query = db_select('lab_migration_proposal');
    //Select fields here all.
    $query->fields('lab_migration_proposal');
    //Order the rows by id in descending.
    $query->orderBy('id', 'DESC');
    //Execute and fetch the rows from db.
    $proposal_q = $query->execute();
    //Create a div element hidden..
    $contents .= '<div id="selected_proposal" style="display: none"><h4>Selected Proposal</h4>';
    //Get the empty progressBar and append to contents.
    $contents .= user_lab_migration_proposals_progress();
    //Display a heading 'All Proposals'.
    $contents .= '<h4>All Proposals</h4>';

    //Iterate over all rows fetched from db
    while ($proposal_data = $proposal_q->fetchObject()) {
      //Get approval status
      $approval_status = getApprovalStatus($proposal_data->approval_status);
      $proposal_rows[] = array(
        $proposal_data->id,
        date('d-m-Y', $proposal_data->creation_date),
        $proposal_data->lab_title,
        $proposal_data->department,
        $approval_status,
        //Display a link to fill display and fill the progressBar.
        '<a href="#" onclick=selectProposal_LM(' . $proposal_data->approval_status . ')>Select</a>',
      );
    }
    //Check rows fetched from db.
    if (!$proposal_rows) {
      //If no row is fetch then return content.
      return $contents .= t('There are no proposal.');
    }
    //Headers for html table.
    $proposal_header = array(
      'Proposal ID',
      'Date of Submission',
      'Title of the Lab',
      'Department',
      'Status',
      'Select Proposal',
    );
    //create a html table from rows and headers and append it to contents.
    $contents .= theme('table', array(
        'header' => $proposal_header,
        'rows' => $proposal_rows,
      ));
    //Return contents to render the html page.
    return $contents;
  }
}

function user_lab_migration_proposals_progress() {
  //Render a empty html progressBar.
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
  //Get approval status of proposal in string.
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

