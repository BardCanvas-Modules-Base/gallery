<?php
/**
 * Settings field type extenders: image selector.
 * Similar to the featured image selector on the posts module.
 * Called from within a function.
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing globals:
 * @var module[] $modules
 * @var module   $current_module settings
 * @var module   $this_module    self
 */

use hng2_base\module;

global $config;

$params = $config->globals["specs_renderer/image_selector"];

$src = empty($params["value"]) 
     ? "{$config->full_root_path}/media/missing_image.png" 
    : $params["value"];
?>

<div class="image_selector_container" align="center">
    <input type="hidden" class="src_image_value" name="names[<?= $params["name"] ?>]" value="<?= $params["value"] ?>">
    
    <a class="prettyPhoto" href="<?= $src ?>"><img class="settings_image_selector"
       src="<?= $src ?>" data-empty-src="<?= $config->full_root_path ?>/media/missing_image.png"></a><br>
    
    <span class="pseudo_link" onclick="set_image(this)">
        <?= $this_module->language->settings_image_selector->set ?>
    </span>
    &nbsp;
    <span class="pseudo_link" onclick="remove_image(this)">
        <?= $this_module->language->settings_image_selector->remove ?>
    </span>
</div>
