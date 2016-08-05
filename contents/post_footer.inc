<?php
/**
 * @var settings $settings
 */

use hng2_base\settings;
?>

<!-- gallery :: post footer strings for tinymce -->

<div id="gallery_strings_for_tinymce" style="display: none;">
    <div class="max_item_width"><?= (int) $settings->get("modules:gallery.max_item_width") ?></div>
    <div class="browser_dialog_title"><?= $this_module->language->tinymce_strings->browser_dialog_title ?></div>
    <div class="media_type_not_supported"><?= $this_module->language->tinymce_strings->media_type_not_supported ?></div>
    <div class="invalid_type_selected"><?= $this_module->language->tinymce_strings->invalid_type_selected ?></div>
    <div class="image_button"><?= $this_module->language->tinymce_strings->insert_image ?></div>
    <div class="video_button"><?= $this_module->language->tinymce_strings->insert_video ?></div>
</div>
