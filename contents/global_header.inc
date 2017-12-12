<?php
/**
 * @var config $config
 */

use hng2_base\config;
?>

<!-- gallery :: html head include -->

<script type="text/javascript"
        src="<?=$config->full_root_path?>/gallery/media/global_functions~v<?= $this_module->version ?>-1.js"></script>

<!--suppress CssUnusedSymbol -->
<style type="text/css">
    /* TinyMCE Exceptions */
    .tinymce_container[data-tinymce-hidden-buttons*="gallery-module"] .mce-btn.mce-gallery_module { display: none; }
</style>
