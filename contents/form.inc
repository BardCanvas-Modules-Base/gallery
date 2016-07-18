<?php
/**
 * Gallery editing form
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_modules\categories\categories_repository;

$categories_repository = new categories_repository();
$default_category      = $categories_repository->get("0000000000000");

$tree_filter = array(
    "id_category <> '0000000000000'",
    "( visibility = 'public' or visibility = 'users' or (visibility  = 'level_based' and '{$account->level}' >= min_level) )"
);
$categories_count = $categories_repository->get_record_count($tree_filter);

$max_upload_size = $settings->get("engine.max_upload_size", 10);
$allowed_types   = array_keys($config->upload_file_types);

$types_list = array();
foreach($allowed_types as $type) $types_list[] = "<code>$type</code>";
$types_list = implode(", ", $types_list);
?>

<div id="media_form_target" style="display: none;"></div>

<form name="media_form" id="media_form" method="post" enctype="multipart/form-data"
      action="<?= $config->full_root_path ?>/gallery/scripts/save.php">
    
    <input type="hidden" name="MAX_FILE_SIZE" value="<?= $max_upload_size * 1024 * 1024 ?>">
    <input type="hidden" name="id_media" value="">
    <input type="hidden" name="status" value="draft">
    
    <div class="field" data-field="title">
        <textarea name="title" class="expandible_textarea no_fw_font" 
                  placeholder="<?= $current_module->language->form->fields->title->placeholder ?>"></textarea>
    </div>
    
    <? $style = $categories_count == 0 ? "display: none" : ""; ?>
    <div class="field" id="main_category_selector_container" data-field="category" style="<?= $style ?>">
        <select name="main_category" class="fixed_font">
            <option selected value="<?= $default_category->id_category ?>">
                <?= $default_category->title ?>: <?= $default_category->description ?>
            </option>
        </select>
    </div>
    
    <div class="field" data-field="file">
        <div class="caption"><?= $current_module->language->form->fields->file->caption ?></div>
        <div class="input">
            <input type="file" name="file">
        </div>
        <div class="input info_handler">
            <span class="fa fa-info-circle"></span>
            <?= replace_escaped_vars(
                $current_module->language->form->fields->file->info,
                array('{$size}', '{$types}'),
                array($max_upload_size, $types_list)
            ) ?>
        </div>
    </div>
    
    <div class="field" data-field="description">
        <div class="caption"><?= $current_module->language->form->fields->description->caption ?></div>
        <div class="input">
            <textarea name="description" id="media_description_editor" class="tinymce"></textarea>
        </div>
    </div>
    
    <p class="post_buttons" style="text-align: right;">
        <button type="reset" onclick="hide_media_form()">
            <span class="fa fa-ban"></span>
            <?= $language->cancel ?>
        </button>
        
        <button type="submit" data-save-type="save_draft"
                onclick="$('#media_form').find('input[name=status]').val('draft')">
            <span class="fa fa-check"></span>
            <?= $current_module->language->form->buttons->save_draft ?>
        </button>
        <button type="submit" data-save-type="save">
            <span class="fa fa-check"></span>
            <?= $language->save ?>
        </button>
        <button type="submit" data-save-type="publish"
                onclick="$('#media_form').find('input[name=status]').val('published')">
            <span class="fa fa-check"></span>
            <?= $current_module->language->form->buttons->publish ?>
        </button>
    </p>
    
</form>
