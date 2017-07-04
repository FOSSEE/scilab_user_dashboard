<?php

/**
 * @file
 * User can history of Book proposals.
 */
function user_tbc_manage_proposals() {
  //If user has permission to access.
  if (user_access('access user_dash_tbc manage')) {
    //Create a empty string for render html.
    $contents = '';
    //Empty array for getting results/row from db
    $book_rows = array();
    //Get rows from {textbook_companion_proposal} and {textbook_companion_preference} by inner join.
    $result = db_query('SELECT textbook_companion_proposal.id,
							textbook_companion_preference.book_status,
							textbook_companion_proposal.new_proposal_status,
							textbook_companion_preference.book,
							textbook_companion_preference.edition,
							textbook_companion_preference.author,
							textbook_companion_preference.year
							FROM {textbook_companion_proposal} INNER JOIN {textbook_companion_preference} ON textbook_companion_preference.proposal_id=textbook_companion_proposal.id
							ORDER BY textbook_companion_proposal.id DESC;');
    //Add a div element which not visible.
    $contents .= '<div id="selected_proposal" style="display:none;"><h4>Current Proposal</h4>';
    //   dpm(get_defined_vars());
    //Get empty progressBar.
    $contents .= user_tbc_progress();
    //Displays the heading 'All Proposals'.
    $contents .= '<h4>All Proposals</h4>';

    //Iterate over all the results/row provide by db
    while ($book_data = $result->fetchObject()) {
      //Get status of book.
      $book_status = getBookStatus($book_data->book_status);
      //Get status of proposal.
      $proposal_status = getProposalStatus($book_data->new_proposal_status);
      //Store data from db in table row for html rendering.
      $book_rows[] = array(
        $book_data->id,
        $book_data->book . '<br> <em>by ' . $book_data->author . '</em>',
        $book_data->edition,
        $book_data->year,
        $proposal_status,
        $book_status,
        //Create a html link for calling javascript function to display and fill the progressBar.
        '<a href="#" onclick="selectProposal_TBC(' . $book_data->new_proposal_status . ',' . $book_data->book_status . ');">Select</a>',
      );
    }
    if (!$book_rows) {
      //If no row is fetched. return the contents.
      return $contents .= t('There are no proposal.');
    }

    //Heading for html table
    $book_header = array(
      'Proposal ID',
      'Book',
      'Edition',
      'Year',
      'Proposal Status',
      'Book Status',
      'Select Proposal',
    );
    //Create a html table from header and rows.
    $contents .= theme('table', array(
        'header' => $book_header,
        'rows' => $book_rows,
      ));
    //Return the contents to render the all to html page.
    return $contents;
  }
}

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

function getProposalStatus($proposal_status = 0) {
  //Gets the proposal status
  switch ($proposal_status) {
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

function getBookStatus($book_status = 0) {
  //Gets the book status
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

