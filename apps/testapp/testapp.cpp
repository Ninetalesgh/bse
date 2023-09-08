

void initialize( bse::PlatformInitParams* )
{
  log_info( "Initialized nothing." );
}

void on_reload( bse::Platform* )
{
  log_info( "Reloaded nothing." );
}

void tick( bse::Platform* pf )
{
  log_info( "testapp!. dt: ", pf->thisFrame.deltaTime, "\n- - frame index: ", pf->thisFrame.frameIndex );
}