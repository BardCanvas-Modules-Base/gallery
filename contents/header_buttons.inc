<?php
/**
 * Media header buttons
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

if ( ! $account->_exists) return;
if( $account->state != "enabled" ) return;

$this_module->load_extensions("header_buttons", "prechecks");
if( $config->globals["@gallery:abort_menu_buttons_load"] ) return;

$added_class = $current_module->name == $this_module->name ? "current" : "";
if( $template->get("showing_archive") ) $added_class = "";

$wasuuup     = md5(mt_rand(1, 65535));
$href        = $_SERVER["REQUEST_URI"];

$icon = empty($this_module->language->icon) ? "" : ("<span class='fa " . trim($this_module->language->icon) . "'></span>");

$template->add_menu_item(
    trim($this_module->language->index->menu_caption),
    "
    <a class='main_menu_item {$added_class}' data-module='gallery' href='{$config->full_root_path}/gallery/'>
        $icon {$this_module->language->index->menu_caption}
    </a>
    ",
    200
);
