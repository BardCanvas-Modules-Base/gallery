
function prepare_media_addition()
{
    var $workarea = $('#form_workarea');
    $workarea.find('.for_edition').hide();
    $workarea.find('.for_addition').show();
    
    var $form = $('#media_form');
    $form.find('.post_buttons button[data-save-type="save_draft"]').show();
    $form.find('.post_buttons button[data-save-type="save"]').hide();
    $form.find('.post_buttons button[data-save-type="publish"]').show();
    $form.find('.field[data-field="file"]').show();
    
    reset_media_form();
    show_media_form();
    update_category_selector();
}

function edit_media(id_media)
{
    var $workarea = $('#form_workarea');
    $workarea.find('.for_edition').show();
    $workarea.find('.for_addition').hide();
    
    var url    = $_FULL_ROOT_PATH + '/gallery/scripts/get_as_json.php';
    var params = {
        'id_media': id_media,
        'wasuuup':  parseInt(Math.random() * 1000000000000000)
    };
    
    $.blockUI(blockUI_default_params);
    $.getJSON(url, params, function(data)
    {
        if( data.message != 'OK' )
        {
            $.unblockUI();
            alert(data.message);
            
            return;
        }
        
        var record = data.data;
        var $form  = $('#media_form');
        
        $form.find('.post_buttons button[data-save-type="save_draft"]').hide();
        $form.find('.post_buttons button[data-save-type="save"]').show();
        $form.find('.field[data-field="file"]').hide();
        
        if( record.status == 'draft' )
            $form.find('.post_buttons button[data-save-type="publish"]').show();
        else
            $form.find('.post_buttons button[data-save-type="publish"]').hide();
        
        reset_media_form();
        fill_media_form($form, record);
        $.unblockUI();
        show_media_form();
        update_category_selector(record.main_category);
    });
}

/**
 * 
 * @param {jQuery} $form
 * @param {object} record
 */
function fill_media_form($form, record)
{
    $form.find('input[name="id_media"]').val( record.id_media );
    $form.find('input[name="status"]').val( record.status );
    $form.find('textarea[name="title"]').val( record.title );
    
    var editor = tinymce.get('media_description_editor');
    editor.setContent(record.description, {format : 'raw'});
}

function update_category_selector(preselected_id)
{
    if( typeof preselected_id == 'undefined' ) preselected_id = '';
    
    var $container = $('#main_category_selector_container');
    $container.block(blockUI_smallest_params);
    
    var url = $_FULL_ROOT_PATH + '/categories/scripts/tree_as_json.php'
            + '?with_description=true'
            + '&wasuuup=' + parseInt(Math.random() * 1000000000000000);
    $.getJSON(url, function(data)
    {
        if( data.message != 'OK' )
        {
            alert(data.message);
            $container.unblock();
            
            return;
        }
        
        var $select = $container.find('select');
        $select.find('option').remove();
        
        var selected;
        for( var key in data.data )
        {
            selected = key == preselected_id ? 'selected' : '';
            $select.append('<option ' + selected + ' value="' + key + '">' + data.data[key] + '</option>');
        }
        
        $container.unblock();
    });
}

function trash_media(id_media)
{
    var url = $_FULL_ROOT_PATH + '/gallery/scripts/trash.php';
    var params = {
        'id_media': id_media,
        'wasuuup':  parseInt(Math.random() * 1000000000000000)
    };
    
    var $row = $('#media_browser_table').find('tr[data-record-id="' + id_media + '"]');
    
    $row.block(blockUI_smallest_params);
    $.get(url, params, function(response)
    {
        if( response != 'OK' )
        {
            alert(response);
            $row.unblock();
            
            return;
        }
        
        $row.unblock();
        $('#refresh_media_browser').click();
    });
}

function reset_media_form()
{
    var $form = $('#media_form');
    $form.find('input[name="id_media"]').val('');
    $form[0].reset();
}

function show_media_form()
{
    $('#main_workarea').hide('fast');
    $('#form_workarea').show('fast');
}

function hide_media_form()
{
    $('#form_workarea').hide('fast');
    $('#main_workarea').show('fast');
}

function prepare_media_form_serialization()
{
    var $form = $('#media_form');
    $form.find('textarea.tinymce').each(function()
    {
        var id      = $(this).attr('id');
        var editor  = tinymce.get(id);
        var content = editor.getContent();
        console.log( content );
        $(this).val( content );
    });
}

function prepare_media_form_submission()
{
    $.blockUI(blockUI_default_params);
}

function process_media_form_response(response)
{
    $.unblockUI();
    if( response != 'OK' )
    {
        alert( response );
        return;
    }
    
    hide_media_form();
    $('#refresh_media_browser').click();
}

$(document).ready(function()
{
    $('#media_form').ajaxForm({
        target:          '#media_form_target',
        beforeSerialize: prepare_media_form_serialization,
        beforeSubmit:    prepare_media_form_submission,
        success:         process_media_form_response
    });
});
