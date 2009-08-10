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
                <p>Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. </p>
              </div>

              <div id="account" class="left">
                <?php 
									if($account){
											print $account;
										}else{
								?>	

									<!-- account profile-->									
									<div id="account-profile" class="clearfix">
										<div class="user">

											<div class="logout">
												<?php print l(t('log out'), 'logout', $options= array('attributes' => array('class' =>'logout')) );  ?>	
											</div>

											<h5>Velkommen</h5>
											<div class="username">
												<?php print l($user->name, 'user/'.$user->uid.'/edit', $options= array('attributes' => array('class' =>'username')) );  ?>												
											</div>


										</div>

										<div class="cart">
											<div>5</div>
											<a href="#">gå til kurv</a>
										</div>

										<ul>
											<li>
												<div class="content">Lånte materialer<span>5</span></div>
												<div class="status"><span class="warning">!!</span></div>
											</li>
											<li>
												<div class="content">Reservationer<span>505</span></div>
												<div class="status"><span class="ok">ok</span></div>
											</li>
										</ul>
									</div>
									<!-- account profile-->
								<?php	} //end account ?>

									 		
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

                <?php print $content; ?>

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