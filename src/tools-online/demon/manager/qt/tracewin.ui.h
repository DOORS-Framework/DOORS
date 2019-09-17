void TraceWin::refreshButton()
{
  std::cout << "void TraceWin::refreshButton()" << std::endl;
  traceView->reTrace();
}

void TraceWin::closeButton()
{
  hide();
}
