# Providers describe their settings declaratively

A Provider shows its settings in Blabby's Settings screen by describing them as a typed list (switch, choice,
number, text, action), which the shell draws with its own controls. Providers do not ship QML, so every settings
page looks and behaves the same on the touch display, plugins do not depend on the shell's QML components, and a
Provider's settings can be tested without a UI.

## Considered Options

- **Plugin-supplied QML**: the Provider hands the shell a QML component for its page. Most freedom, but every plugin
  couples to the shell's styling and components and can break or diverge from it.
- **Declarative with a QML escape hatch**: rejected for now; it can be added later if a real Provider needs a page
  the typed list cannot express.
