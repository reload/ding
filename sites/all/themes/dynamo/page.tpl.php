<?php // dsm(get_defined_vars()) ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<!--
  Dynamo!
-->
<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $body_classes; ?>">


<?php 
/*adds support for for the admin module*/
 if (!empty($admin)) print $admin; 
?>

<div id="container" class="clearfix">

    <div id="page" class="minheight">
      <div id="page-inner" class="clearfix">

        <<?php print $site_name_element; ?> id="site-name">
          <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home">
            <?php print $site_name; ?>
          </a>
        </<?php print $site_name_element; ?>>

        <div id="pageheader">
          <div id="pageheader-inner">
            
            <div id="top" class="clearfix">

              <div id="search" class="left">
                <?php print $search ?>
              </div>

              <div id="account" class="left">
                <?php print $account; ?>	
              </div>  

            </div>

        		<div id="navigation">
        			<div id="navigation-inner">
                <?php if ($primary_links){ ?>
                  <?php print theme('links', $primary_links); ?>
                <?php } ?>
              </div>
            </div>

            <?php if ($breadcrumb){ ?>
          		<div id="path">
          		   <?php print t('You are here:') ." ". $breadcrumb; ?> 
              </div>
            <?php } ?>

          </div>
        </div>
        
        <div id="pagebody" class="clearfix">
          <div id="pagebody-inner" class="clearfix">

            <?php if ($left) { ?>
              <div id="content-left">
                <?php print $left; ?>
              </div>
            <?php } ?>

          	<div id="content">
              <div id="content-inner">

    						<?php if ($help OR $messages) { ?>
	          			<div id="drupal-messages">
	                	  <?php print $help ?>
	                	  <?php print $messages ?>
	          			</div>
								<?php } ?>
								
								<div id="content-main">
									<?php print $content; ?>	
								</div>
                

                <?php if ($tabs){ ?>
                  <div class="tabs"><?php print $tabs; ?></div>
                <?php }; ?>
             
              </div>
          	</div>

            <?php if ($right) { ?>
              <div id="content-right">
                <?php print $right; ?>
              </div>
            <?php } ?>

          </div>
        </div>

        <div id="pagefooter">
          <div id="pagefooter-inner" class="clearfix">

            <div class="left first">
              <?php print $footer_one; ?>
            </div>

            <div class="left">
              <?php print $footer_two; ?>
            </div>

            <div class="left">
              <?php print $footer_three; ?>             
            </div>

            <div class="left">
              <?php print $footer_four; ?>              
              <?php print $footer; ?>
            </div>
      
          </div>
        </div>

      </div>
    </div>

</div>

<?php print $closure; ?>
</body>
</html>
